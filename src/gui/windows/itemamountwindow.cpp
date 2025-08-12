/*
 *  The ManaVerse Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2020  The ManaPlus Developers
 *  Copyright (C) 2020-2025  The ManaVerse Developers
 *
 *  This file is part of The ManaVerse Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gui/windows/itemamountwindow.h"

#include "settings.h"

#include "being/playerinfo.h"

#include "input/keyboardconfig.h"

#include "gui/viewport.h"

#include "gui/models/itemsmodel.h"

#include "gui/popups/itempopup.h"

#include "gui/windows/maileditwindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/tradewindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/icon.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"
#include "gui/widgets/slider.h"

#include "net/inventoryhandler.h"
#include "net/mail2handler.h"
#include "net/npchandler.h"

#include "resources/item/item.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#ifndef USE_SDL2
#include <cmath>
#endif  // USE_SDL2

#include "debug.h"

void ItemAmountWindow::finish(Item *const item,
                              const int amount,
                              const int price,
                              const ItemAmountWindowUsageT usage)
{
    if (item == nullptr)
        return;
    switch (usage)
    {
        case ItemAmountWindowUsage::TradeAdd:
            if (tradeWindow != nullptr)
                tradeWindow->tradeItem(item, amount, false);
            break;
        case ItemAmountWindowUsage::ItemDrop:
            PlayerInfo::dropItem(item, amount, Sfx_true);
            break;
        case ItemAmountWindowUsage::ItemSplit:
            inventoryHandler->splitItem(item, amount);
            break;
        case ItemAmountWindowUsage::StoreAdd:
            inventoryHandler->moveItem2(InventoryType::Inventory,
                item->getInvIndex(), amount, InventoryType::Storage);
            break;
        case ItemAmountWindowUsage::StoreRemove:
            inventoryHandler->moveItem2(InventoryType::Storage,
                item->getInvIndex(), amount, InventoryType::Inventory);
            break;
        case ItemAmountWindowUsage::ShopBuyAdd:
            if (shopWindow != nullptr)
                shopWindow->addBuyItem(item, amount, price);
            break;
        case ItemAmountWindowUsage::ShopSellAdd:
            if (shopWindow != nullptr)
                shopWindow->addSellItem(item, amount, price);
            break;
        case ItemAmountWindowUsage::CartAdd:
            inventoryHandler->moveItem2(InventoryType::Inventory,
                item->getInvIndex(), amount, InventoryType::Cart);
            break;
        case ItemAmountWindowUsage::CartRemove:
            inventoryHandler->moveItem2(InventoryType::Cart,
                item->getInvIndex(), amount, InventoryType::Inventory);
            break;
        case ItemAmountWindowUsage::MailAdd:
            if (settings.enableNewMailSystem)
            {
                mail2Handler->addItem(item, amount);
            }
            else if (mailEditWindow != nullptr)
            {
                mailEditWindow->addItem(item, amount);
            }
            break;
        case ItemAmountWindowUsage::CraftAdd:
        {
            NpcDialog *const dialog = npcHandler->getCurrentNpcDialog();
            if (dialog != nullptr)
                dialog->addCraftItem(item, amount, price);  // price as slot
            break;
        }
        default:
            break;
    }
}

ItemAmountWindow::ItemAmountWindow(const ItemAmountWindowUsageT usage,
                                   Window *const parent,
                                   Item *const item,
                                   const int maxRange) :
    Window("", Modal_false, parent, "amount.xml"),
    ActionListener(),
    KeyListener(),
    mItemAmountTextField(new IntTextField(this, 1, 0, 0, Enable_true, 0)),
    mItemPriceTextField(nullptr),
    mGPLabel(nullptr),
    mItem(item),
    mItemIcon(new Icon(this,
        item != nullptr ? item->getImage() : nullptr, AutoRelease_false)),
    mItemAmountSlide(new Slider(this, 1.0, maxRange, 1.0)),
    mItemPriceSlide(nullptr),
    mItemDropDown(nullptr),
    mItemsModal(nullptr),
    mPrice(0),
    mMax(maxRange),
    mUsage(usage),
    mEnabledKeyboard(keyboard.isEnabled())
{
    if (mItem == nullptr)
        return;

    if (usage == ItemAmountWindowUsage::ShopBuyAdd)
        mMax = 10000;
    else if (mMax == 0)
        mMax = mItem->getQuantity();

    keyboard.setEnabled(false);

    mItemAmountTextField->setRange(1, mMax);
    mItemAmountTextField->setWidth(35);
    mItemAmountTextField->addKeyListener(this);

    mItemAmountSlide->setHeight(10);
    mItemAmountSlide->setActionEventId("slide");
    mItemAmountSlide->addActionListener(this);

    if (mUsage == ItemAmountWindowUsage::ShopBuyAdd ||
        mUsage == ItemAmountWindowUsage::ShopSellAdd)
    {
        mItemPriceTextField = new IntTextField(this, 1, 0, 0, Enable_true, 0);
        mItemPriceTextField->setRange(1, 10000000);
        mItemPriceTextField->setWidth(35);
        mItemPriceTextField->addKeyListener(this);

        mItemPriceSlide = new Slider(this, 1.0, 10000000, 1.0);
        mItemPriceSlide->setHeight(10);
        mItemPriceSlide->setActionEventId("slidePrice");
        mItemPriceSlide->addActionListener(this);

        mGPLabel = new Label(this, " GP");
    }

    if (mUsage == ItemAmountWindowUsage::ShopBuyAdd)
    {
        mItemsModal = new ItemsModal;
        mItemDropDown = new DropDown(this,
            mItemsModal,
            false,
            Modal_false,
            nullptr,
            std::string());
        mItemDropDown->setActionEventId("itemType");
        mItemDropDown->addActionListener(this);
    }

    // Buttons
    Button *const minusAmountButton = new Button(this,
        // TRANSLATORS: item amount window button
        _("-"),
        "dec",
        BUTTON_SKIN,
        this);
    Button *const plusAmountButton = new Button(this,
        // TRANSLATORS: item amount window button
        _("+"),
        "inc",
        BUTTON_SKIN,
        this);
    Button *const okButton = new Button(this,
        // TRANSLATORS: item amount window button
        _("OK"),
        "ok",
        BUTTON_SKIN,
        this);
    Button *const cancelButton = new Button(this,
        // TRANSLATORS: item amount window button
        _("Cancel"),
        "cancel",
        BUTTON_SKIN,
        this);
    Button *const addAllButton = new Button(this,
        // TRANSLATORS: item amount window button
        _("All"),
        "all",
        BUTTON_SKIN,
        this);

    minusAmountButton->adjustSize();
    minusAmountButton->setWidth(plusAmountButton->getWidth());

    // Set positions
    ContainerPlacer placer(nullptr, nullptr);
    placer = getPlacer(0, 0);
    int n = 0;
    if (mUsage == ItemAmountWindowUsage::ShopBuyAdd)
    {
        placer(0, n, mItemDropDown, 8, 1);
        n++;
    }
    placer(1, n, minusAmountButton, 1, 1);
    placer(2, n, mItemAmountTextField, 3, 1);
    placer(5, n, plusAmountButton, 1, 1);
    placer(6, n, addAllButton, 1, 1);

    placer(0, n, mItemIcon, 1, 3);
    placer(1, n + 1, mItemAmountSlide, 7, 1);

    if (mUsage == ItemAmountWindowUsage::ShopBuyAdd ||
        mUsage == ItemAmountWindowUsage::ShopSellAdd)
    {
        Button *const minusPriceButton = new Button(this,
            // TRANSLATORS: item amount window button
            _("-"),
            "decPrice",
            BUTTON_SKIN,
            this);
        Button *const plusPriceButton = new Button(this,
            // TRANSLATORS: item amount window button
            _("+"),
            "incPrice",
            BUTTON_SKIN,
            this);
        minusPriceButton->adjustSize();
        minusPriceButton->setWidth(plusPriceButton->getWidth());

        placer(1, n + 2, minusPriceButton, 1, 1);
        placer(2, n + 2, mItemPriceTextField, 3, 1);
        placer(5, n + 2, plusPriceButton, 1, 1);
        placer(6, n + 2, mGPLabel, 1, 1);

        placer(1, n + 3, mItemPriceSlide, 7, 1);
        placer(4, n + 5, cancelButton, 1, 1);
        placer(5, n + 5, okButton, 1, 1);
    }
    else
    {
        placer(4, n + 2, cancelButton, 1, 1);
        placer(5, n + 2, okButton, 1, 1);
    }

    reflowLayout(225, 0);

    resetAmount();

    switch (usage)
    {
        case ItemAmountWindowUsage::TradeAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to trade."));
            break;
        case ItemAmountWindowUsage::ItemDrop:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to drop."));
            break;
        case ItemAmountWindowUsage::StoreAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to store."));
            break;
        case ItemAmountWindowUsage::MailAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to send."));
            break;
        case ItemAmountWindowUsage::CraftAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to craft."));
            break;
        case ItemAmountWindowUsage::CartAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to store to cart."));
            break;
        case ItemAmountWindowUsage::StoreRemove:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to retrieve."));
            break;
        case ItemAmountWindowUsage::CartRemove:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to retrieve from cart."));
            break;
        case ItemAmountWindowUsage::ItemSplit:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to split."));
            break;
        case ItemAmountWindowUsage::ShopBuyAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Add to buy shop."));
            break;
        case ItemAmountWindowUsage::ShopSellAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Add to sell shop."));
            break;
        default:
            // TRANSLATORS: amount window message
            setCaption(_("Unknown."));
            break;
    }

    setLocationRelativeTo(getParentWindow());

    mItemIcon->addMouseListener(this);
}

void ItemAmountWindow::postInit()
{
    Window::postInit();
    setVisible(fromBool(mItem, Visible));
}

ItemAmountWindow::~ItemAmountWindow()
{
    delete2(mItemsModal)
}

// Show ItemTooltip
void ItemAmountWindow::mouseMoved(MouseEvent &event)
{
    Window::mouseMoved(event);

    if ((viewport == nullptr) || (itemPopup == nullptr))
        return;

    if (event.getSource() == mItemIcon)
    {
        itemPopup->setItem(mItem, false);
        itemPopup->position(viewport->mMouseX, viewport->mMouseY);
    }
}

// Hide ItemTooltip
void ItemAmountWindow::mouseExited(MouseEvent &event A_UNUSED)
{
    if (itemPopup != nullptr)
        itemPopup->setVisible(Visible_false);
}

void ItemAmountWindow::resetAmount()
{
    mItemAmountTextField->setValue(1);
}

void ItemAmountWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "cancel")
    {
        close();
        return;
    }
    else if (eventId == "ok")
    {
        if (mItemPriceTextField != nullptr)
        {
            finish(mItem,
                mItemAmountTextField->getValue(),
                mItemPriceTextField->getValue(),
                mUsage);
        }
        else
        {
            if (mUsage == ItemAmountWindowUsage::CraftAdd)
            {
                finish(mItem,
                    mItemAmountTextField->getValue(),
                    mPrice,
                    mUsage);
            }
            else
            {
                finish(mItem,
                    mItemAmountTextField->getValue(),
                    0,
                    mUsage);
            }
        }
        close();
        return;
    }
    else if (eventId == "itemType")
    {
        if ((mItemDropDown == nullptr) || (mItemsModal == nullptr))
            return;

        const int id = ItemDB::get(mItemsModal->getElementAt(
            mItemDropDown->getSelected())).getId();

        mItem = new Item(id,
            ItemType::Unknown,
            10000,
            0,
            ItemColor_one,
            Identified_true,
            Damaged_true,
            Favorite_false,
            Equipm_false,
            Equipped_false);

        if (mUsage == ItemAmountWindowUsage::ShopBuyAdd)
            mMax = 10000;
        else if (mMax == 0)
            mMax = mItem->getQuantity();

        mItemIcon->setImage(mItem->getImage());
    }

    int amount = mItemAmountTextField->getValue();

    if (eventId == "inc" && amount < mMax)
        amount++;
    else if (eventId == "dec" && amount > 1)
        amount--;
    else if (eventId == "all")
        amount = mMax;
    else if (eventId == "slide")
        amount = CAST_S32(mItemAmountSlide->getValue());
    mItemAmountTextField->setValue(amount);
    mItemAmountSlide->setValue(amount);

    if ((mItemPriceTextField != nullptr) && (mItemPriceSlide != nullptr))
    {
        if (mPrice > 7)
            mPrice = 7;
        else if (mPrice < 0)
            mPrice = 0;

        int price = 0;

        if (eventId == "incPrice")
        {
            mPrice++;
            price = CAST_S32(pow(10.0, mPrice));
            mItemPriceTextField->setValue(price);
            mItemPriceSlide->setValue(price);
        }
        else if (eventId == "decPrice")
        {
            mPrice--;
            price = CAST_S32(pow(10.0, mPrice));
            mItemPriceTextField->setValue(price);
            mItemPriceSlide->setValue(price);
        }
        else if (eventId == "slidePrice")
        {
            price = CAST_S32(mItemPriceSlide->getValue());
            if (price != 0)
                mPrice = CAST_S32(log(static_cast<float>(price)));
            else
                mPrice = 0;
            mItemPriceTextField->setValue(price);
            mItemPriceSlide->setValue(price);
        }
    }
}

void ItemAmountWindow::close()
{
    keyboard.setEnabled(mEnabledKeyboard);
    scheduleDelete();
}

void ItemAmountWindow::keyReleased(KeyEvent &event A_UNUSED)
{
    mItemAmountSlide->setValue(mItemAmountTextField->getValue());
}

void ItemAmountWindow::showWindow(const ItemAmountWindowUsageT usage,
                                  Window *const parent,
                                  Item *const item,
                                  int maxRange,
                                  int tag)
{
    if (item == nullptr)
        return;

    if (maxRange == 0)
        maxRange = item->getQuantity();

    if (usage != ItemAmountWindowUsage::ShopBuyAdd &&
        usage != ItemAmountWindowUsage::ShopSellAdd &&
        maxRange <= 1)
    {
        if (usage == ItemAmountWindowUsage::CraftAdd)
            finish(item, maxRange, tag, usage);
        else
            finish(item, maxRange, 0, usage);
    }
    else
    {
        ItemAmountWindow *const window = CREATEWIDGETR(ItemAmountWindow,
            usage, parent, item, maxRange);
        if (usage == ItemAmountWindowUsage::CraftAdd)
            window->mPrice = tag;
    }
}
